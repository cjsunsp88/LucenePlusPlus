/////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2009-2011 Alan Wright. All rights reserved.
// Distributable under the terms of either the Apache License (Version 2.0)
// or the GNU Lesser General Public License.
/////////////////////////////////////////////////////////////////////////////

#ifndef _CUSTOMSCOREQUERY_H
#define _CUSTOMSCOREQUERY_H

#include "Weight.h"
#include "Scorer.h"
#include "CustomScoreProvider.h"

namespace Lucene
{
    // when deprecated methods are removed, do not extend class here, just return new default CustomScoreProvider
    class DefaultCustomScoreProvider : public CustomScoreProvider
    {
    public:
        DefaultCustomScoreProvider(CustomScoreQueryPtr customQuery, IndexReaderPtr reader);
        virtual ~DefaultCustomScoreProvider();
    
        LUCENE_CLASS(DefaultCustomScoreProvider);
    
    protected:
        CustomScoreQueryWeakPtr _customQuery;
    
    public:
        virtual double customScore(int32_t doc, double subQueryScore, Collection<double> valSrcScores);
        virtual double customScore(int32_t doc, double subQueryScore, double valSrcScore);
        virtual ExplanationPtr customExplain(int32_t doc, ExplanationPtr subQueryExpl, Collection<ExplanationPtr> valSrcExpls);
        virtual ExplanationPtr customExplain(int32_t doc, ExplanationPtr subQueryExpl, ExplanationPtr valSrcExpl);
    };
    
    class CustomWeight : public Weight
    {
    public:
        CustomWeight(CustomScoreQueryPtr query, SearcherPtr searcher);
        virtual ~CustomWeight();
        
        LUCENE_CLASS(CustomWeight);
    
    public:
        CustomScoreQueryPtr query;
        SimilarityPtr similarity;
        WeightPtr subQueryWeight;
        Collection<WeightPtr> valSrcWeights;
        bool qStrict;
    
    public:
        virtual QueryPtr getQuery();
        virtual double getValue();
        virtual double sumOfSquaredWeights();
        virtual void normalize(double norm);
        virtual ScorerPtr scorer(IndexReaderPtr reader, bool scoreDocsInOrder, bool topScorer);
        virtual ExplanationPtr explain(IndexReaderPtr reader, int32_t doc);
        virtual bool scoresDocsOutOfOrder();
    
    protected:
        ExplanationPtr doExplain(IndexReaderPtr reader, int32_t doc);
    };
    
    /// A scorer that applies a (callback) function on scores of the subQuery.
    class CustomScorer : public Scorer
    {
    public:
        CustomScorer(SimilarityPtr similarity, IndexReaderPtr reader, CustomWeightPtr weight, ScorerPtr subQueryScorer, Collection<ScorerPtr> valSrcScorers);
        virtual ~CustomScorer();
    
        LUCENE_CLASS(CustomScorer);
    
    protected:
        double qWeight;
        ScorerPtr subQueryScorer;
        Collection<ScorerPtr> valSrcScorers;
        IndexReaderPtr reader;
        CustomScoreProviderPtr provider;
        Collection<double> vScores; // reused in score() to avoid allocating this array for each doc 
        
    public:
        virtual int32_t nextDoc();
        virtual int32_t docID();
        virtual double score();
        virtual int32_t advance(int32_t target);
    };
}

#endif
